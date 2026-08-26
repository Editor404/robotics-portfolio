import torch
import torch.nn as nn
from torchvision import models
from torchvision.models import ConvNeXt_Tiny_Weights


class MyModel(nn.Module):
    def __init__(self, num_classes=4, dropout=0.3, pretrained=False):
        super().__init__()

        weights = ConvNeXt_Tiny_Weights.IMAGENET1K_V1 if pretrained else None
        self.backbone = models.convnext_tiny(weights=weights)

        in_features = self.backbone.classifier[2].in_features
        self.backbone.classifier[2] = nn.Sequential(
            nn.Dropout(dropout),
            nn.Linear(in_features, num_classes),
        )

    def forward(self, x):
        return self.backbone(x)


def get_model():
    """
    학습 및 추론 스크립트에서 모델 객체를 생성할 때 호출하는 함수입니다.
    """
    return MyModel()
